import chai, { assert, expect } from 'chai';
import chaiAsPromised from 'chai-as-promised';

import fs from 'fs';
import path from 'path';
import R from 'ramda';
import shell from 'shelljs';
import * as Loader from '../src/load';
import { getImplTypeByFilename } from '../src/utils';
import libsFixture from './fixtures/libs.json';
import unpacked from './fixtures/unpacked.json';

import {
  fixture,
  expectRejectedWithCode,
} from './utils';
import * as ERROR_CODES from '../src/errorCodes';

chai.use(chaiAsPromised);

const tempDir = './fixtures/workspace';
const sortByPath = R.sortBy(R.prop('path'));
const rmAttachments = R.map(R.dissocPath(['content', 'attachments']));

describe('Loader', () => {
  const workspace = path.resolve(__dirname, tempDir);
  const projectPath = 'awesome-project';

  it('getLocalProjects: return an array of local projects in workspace', () =>
    Loader.getLocalProjects(workspace)
      .then((projects) => {
        expect(projects).to.have.lengthOf(1);
        expect(projects).to.deep.equal([
          {
            path: path.resolve(workspace, projectPath),
            content: {
              authors: [
                'Amperka team',
              ],
              description: '',
              license: '',
              name: 'awesome-project',
              version: '0.0.42',
            },
          },
        ]);
      })
  );

  it('getProjects: return an array of projects in workspace, including libs', () =>
    Loader.getProjects(workspace)
      .then((projects) => {
        expect(projects).to.have.lengthOf(4);
      })
  );
  it('getProjects: reject CANT_ENUMERATE_PROJECTS for non-existent workspace',
    () => expectRejectedWithCode(
      Loader.getProjects(fixture('./notExistWorkspace')),
      ERROR_CODES.CANT_ENUMERATE_PROJECTS
    )
  );

  it('getLocalProjects: return an array of local projects in workspace', () =>
    Loader.getLocalProjects(workspace)
      .then((projects) => {
        expect(projects).to.have.lengthOf(1);
      })
  );
  it('getLocalProjects: reject CANT_ENUMERATE_PROJECTS for non-existent workspace',
    () => expectRejectedWithCode(
      Loader.getLocalProjects(fixture('./notExistWorkspace')),
      ERROR_CODES.CANT_ENUMERATE_PROJECTS
    )
  );

  it('loadProjectWithLibs: return project with libs', () =>
    Loader.loadProjectWithLibs(projectPath, workspace)
      .then(({ project, libs }) => {
        const quxPatch = R.find(R.pathEq(['content', 'path'], '@/qux'), project);
        const quxPatchFixture = R.find(R.pathEq(['content', 'path'], '@/qux'), unpacked);
        const projectWithoutAttachments = sortByPath(rmAttachments(project));
        const fixtureWithoutAttachments = sortByPath(rmAttachments(unpacked));

        expect(projectWithoutAttachments).to.deep.equal(fixtureWithoutAttachments);
        assert.sameDeepMembers(quxPatch.content.attachments, quxPatchFixture.content.attachments);
        expect(libs).to.deep.equal(libsFixture);
      })
  );

  it('loadProjectWithoutLibs: return project without libs', () => {
    const xodCore = path.resolve(workspace, './lib/xod/core');
    const xodCoreOwner = path.resolve(xodCore, '..');

    return Loader.loadProjectWithoutLibs(xodCore)
      .then((project) => {
        const implList = shell.ls(`${xodCore}/**/*.{c,cpp,h,inl,js}`);
        assert.isAbove(implList.length, 0);

        return implList.every((implPath) => {
          const { base, dir } = path.parse(implPath);
          const implType = getImplTypeByFilename(base);
          const impl = fs.readFileSync(implPath).toString();
          const xodp = `./${path.relative(xodCoreOwner, `${dir}/patch.xodp`)}`;
          const patch = project.find(_ => _.path === xodp);
          if (!(patch && patch.content.impls)) return false;
          return patch.content.impls[implType] === impl;
        });
      }).then(
        implsLoaded => assert.equal(implsLoaded, true, 'some implementations were not loaded')
      );
  });
});
